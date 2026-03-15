from formator import EpubFormator
import sys

editor = EpubFormator(sys.argv[1])
print(editor.data.language)

editor.set_language("en")
print(editor.data.language)
