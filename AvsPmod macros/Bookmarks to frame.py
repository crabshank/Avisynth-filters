import codecs
import os.path

# run in thread
try:
    bookmarks = avsp.GetBookmarkList(title=True)
except TypeError:
    bookmarks = avsp.GetBookmarkList()
bookmarks.sort()
filename = avsp.GetSaveFilename(
                title=_('Save bookmarks list as...'), 
                default=os.path.splitext(
                    avsp.GetScriptFilename(propose='general', only='base'))[0],
                filefilter = '|'.join((_('Text files') + ' (*.txt)|*.txt', 
                                       _('All files') + ' (*.*)|*.*')))
if not filename:
    return
fps = avsp.GetVideoFramerate()
text = []
chapter = 1
for item in bookmarks:
    if type(item) is int:
        bookmark = item

    else:
        bookmark, title = item

    timecode = '%d\n' % (bookmark)

    text += [timecode]

    f = codecs.open(filename, 'w', 'utf-8')
    f.writelines(text)
    f.close()