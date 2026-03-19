from formator import EpubFormator
import sys

book = EpubFormator(sys.argv[1])
# print(book.get_language())

# book.set_language("en")
# print(book.get_language())
# print(book.get_cover())

print(book.data.cover_meta_id)
# print(book.data.manifest)

book.download_cover()
# book.set_cover(sys.argv[2])

