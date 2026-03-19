from .epub_io.container import get_opf_path
from .epub_io.opf import parse_opf, save_opf, get_cover_item
from .operations.metadata import set_language, set_cover, download_cover

class EpubFormator:
    def __init__(self, epub_path: str):
        self.epub_path = epub_path
        self.opf_path  = get_opf_path(epub_path)
        self.data      = parse_opf(epub_path, self.opf_path)

    def get_title(self):
        return self.data.title
    

    def get_author(self):
        return self.data.author


    def get_language(self):
        return self.data.language


    def get_cover(self):
        return self.data.cover_meta_id


    def set_language(self, language: str) -> None:
        set_language(self.data, language)
        save_opf(self.epub_path, self.opf_path, self.data)


    def set_cover(self, cover: str) ->None:
        cover_item = get_cover_item(self.data)
        if cover_item is None:
            raise ValueError("No cover image found in this EPUB")

        set_cover(self.epub_path, cover, cover_item)
        save_opf(self.epub_path, self.opf_path, self.data)


    def download_cover(self) -> None:
        cover_item = get_cover_item(self.data)
        if cover_item is None:
            raise ValueError("No cover image found in this EPUB")

        download_cover(self.epub_path, self.opf_path, cover_item)
