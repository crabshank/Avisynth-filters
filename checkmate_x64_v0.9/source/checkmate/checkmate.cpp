#define NOMINMAX
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include "avisynth.h"
#include <stdint.h>
#include <intrin.h>
#include <algorithm>
#include <emmintrin.h>

template<int minimum, int maximum>
static __forceinline int static_clip(int val) {
    if (val > maximum) {
        return maximum;
    }
    if (val < minimum) {
        return minimum;
    }
    return val;
}

static __forceinline int clip(int val, int minimum, int maximum) {
    if (val > maximum) {
        return maximum;
    }
    if (val < minimum) {
        return minimum;
    }
    return val;
}

static __forceinline __m128i simd_abs_diff_epu16(const __m128i &a, const __m128i &b) {
    __m128i positive = _mm_subs_epu16(a, b);
    __m128i negative = _mm_subs_epu16(b, a);
    return _mm_or_si128(positive, negative);
}

static inline void process_line_sse2(uint8_t *dstp, const uint8_t *srcp_p2, const uint8_t *srcp_p1, const uint8_t *srcp, const uint8_t *srcp_n1, const uint8_t *srcp_n2,
    int src_pitch, int src_p1_pitch, int src_n1_pitch, int width, int tthr, int tmax, int tthr2) {

    __m128i zero = _mm_setzero_si128();
    __m128i tthr_plus_tmax = _mm_adds_epu16(_mm_set1_epi16(tthr), _mm_set1_epi16(tmax));
    __m128i multipler = _mm_set1_epi16((uint16_t)(((1u << 16) + 4) / 10));
    __m128i tmax_multiplier = _mm_set1_epi16((1<<13) / tmax);
    __m128i tmax_vector = _mm_set1_epi16(tmax);
    __m128i tmax_plus_one = _mm_adds_epu16(tmax_vector, _mm_set1_epi16(1));
    __m128i v16384 = _mm_set1_epi16(1 << 14);
    __m128i v8192 = _mm_set1_epi16(8192);
    __m128i tthr2_vector = _mm_set1_epi16(tthr2);

    for (int x = 0; x < width; x += 8) {
        __m128i p1 = _mm_loadl_epi64(reinterpret_cast<const __m128i*>(srcp_p1+x));
        __m128i c = _mm_loadl_epi64(reinterpret_cast<const __m128i*>(srcp+x));
        __m128i n1 = _mm_loadl_epi64(reinterpret_cast<const __m128i*>(srcp_n1+x));

        p1 = _mm_unpacklo_epi8(p1, zero);
        c = _mm_unpacklo_epi8(c, zero);
        n1 = _mm_unpacklo_epi8(n1, zero);

        __m128i next_val = _mm_adds_epu16(c, n1);
        __m128i prev_val = _mm_adds_epu16(c, p1);

        __m128i current_column = _mm_adds_epu16(
            _mm_adds_epu16(c, _mm_unpacklo_epi8(_mm_loadl_epi64(reinterpret_cast<const __m128i*>(srcp + x - src_pitch*2)), zero)),
            _mm_adds_epu16(c, _mm_unpacklo_epi8(_mm_loadl_epi64(reinterpret_cast<const __m128i*>(srcp + x + src_pitch*2)), zero))
            );

        __m128i n1_column = _mm_adds_epu16(
            _mm_adds_epu16(n1, _mm_unpacklo_epi8(_mm_loadl_epi64(reinterpret_cast<const __m128i*>(srcp_n1 + x - src_n1_pitch*2)), zero)),
            _mm_adds_epu16(n1, _mm_unpacklo_epi8(_mm_loadl_epi64(reinterpret_cast<const __m128i*>(srcp_n1 + x + src_n1_pitch*2)), zero))
            );

        __m128i p1_column = _mm_adds_epu16(
            _mm_adds_epu16(p1, _mm_unpacklo_epi8(_mm_loadl_epi64(reinterpret_cast<const __m128i*>(srcp_p1 + x - src_p1_pitch*2)), zero)),
            _mm_adds_epu16(p1, _mm_unpacklo_epi8(_mm_loadl_epi64(reinterpret_cast<const __m128i*>(srcp_p1 + x + src_p1_pitch*2)), zero))
            );

        __m128i nc = _mm_subs_epu16(tthr_plus_tmax, simd_abs_diff_epu16(n1_column, current_column));
        __m128i pc = _mm_subs_epu16(tthr_plus_tmax, simd_abs_diff_epu16(p1_column, current_column));

        //int next_weight = std::min(clip(nc, 0, tmax+1)*tmax_multipler, 8192);
        nc = _mm_mullo_epi16(_mm_min_epu16(nc, tmax_plus_one), tmax_multiplier);
        pc = _mm_mullo_epi16(_mm_min_epu16(pc, tmax_plus_one), tmax_multiplier);

        __m128i next_weight = _mm_min_epi16(nc, v8192);
        __m128i prev_weight = _mm_min_epi16(pc, v8192);
        __m128i curr_weight = _mm_subs_epu16(v16384, _mm_adds_epu16(next_weight, prev_weight));

        __m128i c_12 = _mm_subs_epu16(_mm_slli_epi16(c, 4), _mm_slli_epi16(c, 2));
        __m128i curr_val = _mm_adds_epu16(c_12, _mm_adds_epu16(current_column, current_column));

        __m128i tmp1 = _mm_unpacklo_epi8(_mm_loadl_epi64(reinterpret_cast<const __m128i*>(srcp+x-2-src_pitch*2)), zero);
        __m128i tmp2 = _mm_unpacklo_epi8(_mm_loadl_epi64(reinterpret_cast<const __m128i*>(srcp+x-2)), zero);
        __m128i tmp3 = _mm_unpacklo_epi8(_mm_loadl_epi64(reinterpret_cast<const __m128i*>(srcp+x-2+src_pitch*2)), zero);
        if (x == 0) { 
            tmp1 = _mm_shufflelo_epi16(tmp1, _MM_SHUFFLE(3, 2, 2, 2));
            tmp2 = _mm_shufflelo_epi16(tmp2, _MM_SHUFFLE(3, 2, 2, 2));
            tmp3 = _mm_shufflelo_epi16(tmp3, _MM_SHUFFLE(3, 2, 2, 2));
        }
        curr_val = _mm_subs_epi16(curr_val, tmp1);
        curr_val = _mm_subs_epi16(curr_val, tmp3);
        curr_val = _mm_adds_epi16(curr_val, _mm_adds_epu16(tmp2, tmp2));

        tmp1 = _mm_unpacklo_epi8(_mm_loadl_epi64(reinterpret_cast<const __m128i*>(srcp+x+2-src_pitch*2)), zero);
        tmp2 = _mm_unpacklo_epi8(_mm_loadl_epi64(reinterpret_cast<const __m128i*>(srcp+x+2)), zero);
        tmp3 = _mm_unpacklo_epi8(_mm_loadl_epi64(reinterpret_cast<const __m128i*>(srcp+x+2+src_pitch*2)), zero);
        if (x == (width-8)) {
            tmp1 = _mm_shufflehi_epi16(tmp1, _MM_SHUFFLE(1, 1, 1, 0));
            tmp2 = _mm_shufflehi_epi16(tmp2, _MM_SHUFFLE(1, 1, 1, 0));
            tmp3 = _mm_shufflehi_epi16(tmp3, _MM_SHUFFLE(1, 1, 1, 0));
        }
        curr_val = _mm_subs_epi16(curr_val, tmp1);
        curr_val = _mm_subs_epi16(curr_val, tmp3);
        curr_val = _mm_adds_epi16(curr_val, _mm_adds_epu16(tmp2, tmp2));

        curr_val = _mm_mulhi_epi16(curr_val, multipler);

        __m128i tmp1_weight_lo = _mm_unpacklo_epi16(curr_weight, prev_weight);
        __m128i tmp2_weight_lo = _mm_unpacklo_epi16(next_weight, zero);
        __m128i tmp1_value_lo = _mm_unpacklo_epi16(curr_val, prev_val);
        __m128i tmp2_value_lo = _mm_unpacklo_epi16(next_val, zero);

        __m128i tmp1_lo = _mm_madd_epi16(tmp1_weight_lo, tmp1_value_lo);
        __m128i tmp2_lo = _mm_madd_epi16(tmp2_weight_lo, tmp2_value_lo);
        __m128i sum_lo = _mm_add_epi32(tmp1_lo, tmp2_lo);
        sum_lo = _mm_srai_epi32(sum_lo, 15);

        __m128i tmp1_weight_hi = _mm_unpackhi_epi16(curr_weight, prev_weight);
        __m128i tmp2_weight_hi = _mm_unpackhi_epi16(next_weight, zero);
        __m128i tmp1_value_hi = _mm_unpackhi_epi16(curr_val, prev_val);
        __m128i tmp2_value_hi = _mm_unpackhi_epi16(next_val, zero);

        __m128i tmp1_hi = _mm_madd_epi16(tmp1_weight_hi, tmp1_value_hi);
        __m128i tmp2_hi = _mm_madd_epi16(tmp2_weight_hi, tmp2_value_hi);
        __m128i sum_hi = _mm_add_epi32(tmp1_hi, tmp2_hi);
        sum_hi = _mm_srai_epi32(sum_hi, 15);

        __m128i result = _mm_packs_epi32(sum_lo, sum_hi);
        
        if (tthr2 != 0) {
            __m128i p2 = _mm_unpacklo_epi8(_mm_loadl_epi64(reinterpret_cast<const __m128i*>(srcp_p2+x)), zero);
            __m128i n2 = _mm_unpacklo_epi8(_mm_loadl_epi64(reinterpret_cast<const __m128i*>(srcp_n2+x)), zero);

            __m128i mask1 = _mm_cmplt_epi16(simd_abs_diff_epu16(p1, n1), tthr2_vector);
            __m128i mask2 = _mm_cmplt_epi16(simd_abs_diff_epu16(p2, c), tthr2_vector);
            __m128i mask3 = _mm_cmplt_epi16(simd_abs_diff_epu16(n2, c), tthr2_vector);
            __m128i mask = _mm_and_si128(_mm_and_si128(mask1, mask2), mask3);

            __m128i alt_result = _mm_adds_epu16(
                _mm_adds_epu16(c, p1),
                _mm_adds_epu16(c, n1)
                );
            alt_result = _mm_srli_epi16(alt_result, 2);

            auto t1 = _mm_and_si128(mask, alt_result);
            auto t2 = _mm_andnot_si128(mask, result);
            result = _mm_or_si128(t1, t2);
        }
        result = _mm_packus_epi16(result, zero);
        _mm_storel_epi64(reinterpret_cast<__m128i*>(dstp+x), result);
    }
}

static inline void process_line_c(uint8_t *dstp, const uint8_t *srcp_p2, const uint8_t *srcp_p1, const uint8_t *srcp, const uint8_t *srcp_n1, const uint8_t *srcp_n2,
    int src_pitch, int src_p1_pitch, int src_n1_pitch, int width, int tthr, int tmax, int tthr2) {
    uint16_t tmax_multiplier = ((1<<13) / tmax);
    for (int x = 0; x<width; ++x)
    {
        if (std::abs(srcp_p1[x]-srcp_n1[x])<tthr2 && std::abs(srcp_p2[x]-srcp[x])<tthr2 && std::abs(srcp[x]-srcp_n2[x])<tthr2) {
            dstp[x] = (srcp_p1[x]+srcp[x]*2+srcp_n1[x])>>2;
        } else {
            int next_value = srcp[x]+srcp_n1[x];
            int prev_value = srcp[x]+srcp_p1[x];

            int x_left = x < 2 ? 0 : x - 2;
            int x_right = x > width-3 ? width-1 : x+2;
            /*
            *(-srcp[x_left-src_pitch*2] + srcp[x-src_pitch*2]*2 - srcp[x_right-src_pitch*2]
            * +srcp[x_left]*2	         + srcp[x]*16            + srcp[x_right]*2
            * -srcp[x_left+src_pitch*2] + srcp[x+src_pitch*2]*2 - srcp[x_right+src_pitch*2]) / 10;
            */
            int current_column = srcp[x-src_pitch*2] + srcp[x]*2 + srcp[x+src_pitch*2];
            int curr_value = (-srcp[x_left-src_pitch*2] - srcp[x_right-src_pitch*2]
                +srcp[x_left]*2 + srcp[x_right]*2
                -srcp[x_left+src_pitch*2] - srcp[x_right+src_pitch*2]
                + current_column * 2 + srcp[x]*12) / 10;

            int nc = (srcp_n1[x-src_n1_pitch*2]+srcp_n1[x]*2+srcp_n1[x+src_n1_pitch*2]) - current_column;
            int pc = (srcp_p1[x-src_p1_pitch*2]+srcp_p1[x]*2+srcp_p1[x+src_p1_pitch*2]) - current_column;

            nc = tthr+tmax-std::abs(nc);
            pc = tthr+tmax-std::abs(pc);

            int next_weight = std::min(clip(nc, 0, tmax+1)*tmax_multiplier, 8192);
            int prev_weight = std::min(clip(pc, 0, tmax+1)*tmax_multiplier, 8192);

            int curr_weight = (1<<14) - (next_weight+prev_weight);

            dstp[x] = static_clip<0, 255>((curr_weight*curr_value+prev_weight*prev_value+next_weight*next_value)>>15);
        }
    }
}

class CheckMate : public GenericVideoFilter {
public:
    CheckMate(PClip child, int _tthr, int _tmax, int _tthr2, IScriptEnvironment* env)
        : GenericVideoFilter(child), tthr(_tthr), tmax(_tmax), tthr2(_tthr2) {

        if (!vi.IsPlanar()) {
            env->ThrowError("Checkmate only works with planar colorspaces!");
        }
        if (tmax <= 0 || tmax > 255) {
            env->ThrowError("Checkmate: \"max\" value should be in range [1;255]");
        }
        if (tthr2 < 0) {
            env->ThrowError("Checkmate: thr2 should be non-negative");
        }
    }

    PVideoFrame __stdcall GetFrame(int n, IScriptEnvironment *env);

private:
    int tthr;
    int tmax;
    int tthr2;
};

PVideoFrame CheckMate::GetFrame(int n, IScriptEnvironment *env) {
    if (n < 2 || n > vi.num_frames-2) {
        return child->GetFrame(n, env);
    }

    PVideoFrame src_p2 = tthr2 > 0 ? child->GetFrame(n-2, env) : nullptr;
    PVideoFrame src_p1 = child->GetFrame(n-1, env);
    PVideoFrame src = child->GetFrame(n, env);
    PVideoFrame src_n1 = child->GetFrame(n+1, env);
    PVideoFrame src_n2 = tthr2 > 0 ? child->GetFrame(n+2, env) : nullptr;
    PVideoFrame dst = env->NewVideoFrame(vi);
    
    const static int planes[] = { PLANAR_Y, PLANAR_U, PLANAR_V };
    for (int pid = 0; pid < (vi.IsY8() ? 1 : 3); pid++) {
        int plane = planes[pid];

        int src_p2_pitch = tthr2 > 0 ? src_p2->GetPitch(plane) : 0;
        int src_p1_pitch = src_p1->GetPitch(plane);
        int src_pitch = src->GetPitch(plane);
        int src_n1_pitch = src_n1->GetPitch(plane);
        int src_n2_pitch = tthr2 > 0 ? src_n2->GetPitch(plane) : 0;
        int dst_pitch = dst->GetPitch(plane);

        
        const uint8_t* srcp_p1 = src_p1->GetReadPtr(plane);
        const uint8_t* srcp = src->GetReadPtr(plane);
        const uint8_t* srcp_n1 = src_n1->GetReadPtr(plane);
        const uint8_t* srcp_p2 = tthr2 > 0 ? src_p2->GetReadPtr(plane) : nullptr;
        const uint8_t* srcp_n2 = tthr2 > 0 ? src_n2->GetReadPtr(plane) : nullptr;
        uint8_t* dstp = dst->GetWritePtr(plane);

        int width = src->GetRowSize(plane);
        int height = src->GetHeight(plane);

        env->BitBlt(dstp, dst->GetPitch(plane), srcp, src->GetPitch(plane), width, 2);
        
        srcp_p1 += src_p1_pitch * 2;
        srcp += src_pitch * 2;
        srcp_n1 += src_n1_pitch * 2;
        srcp_p2 += src_p2_pitch * 2;
        srcp_n2 += src_n2_pitch * 2;
        dstp += dst_pitch * 2;
        bool simd = (env->GetCPUFlags() & CPUF_SSE2) && width % 8 == 0;

        for (int y = 2; y<height-2; ++y) {
            if (simd) {
                process_line_sse2(dstp, srcp_p2, srcp_p1, srcp, srcp_n1, srcp_n2, src_pitch, src_p1_pitch, src_n1_pitch, width, tthr, tmax, tthr2);
            } else {
                process_line_c(dstp, srcp_p2, srcp_p1, srcp, srcp_n1, srcp_n2, src_pitch, src_p1_pitch, src_n1_pitch, width, tthr, tmax, tthr2);
            }

            srcp_p2 += src_p2_pitch;
            srcp_p1 += src_p1_pitch;
            srcp += src_pitch;
            srcp_n1 += src_n1_pitch;
            srcp_n2 += src_n2_pitch;
            dstp += dst_pitch;
        }

        env->BitBlt(dstp, dst->GetPitch(plane), srcp, src->GetPitch(plane), width, 2);
    }

    return dst;
}

AVSValue __cdecl create_checkmate(AVSValue args, void* user_data, IScriptEnvironment* env) {
    enum { CLIP, TTHR, TMAX, TTHR2 };
    return new CheckMate(args[CLIP].AsClip(), args[TTHR].AsInt(12), args[TMAX].AsInt(12), args[TTHR2].AsInt(0), env);
}

const AVS_Linkage *AVS_linkage = nullptr;

extern "C" __declspec(dllexport) const char* __stdcall AvisynthPluginInit3(IScriptEnvironment* env, const AVS_Linkage* const vectors) {
    AVS_linkage = vectors;
    env->AddFunction("checkmate", "c[thr]i[max]i[tthr2]i", create_checkmate, 0);
    return "I want a candy";
}