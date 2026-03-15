from epub_io.container import get_opf_path
from epub_io.opf import parse_opf, save_opf
from operations.metadata import set_language

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


    def set_language(self, language: str) -> None:
        set_language(self.data, language)
        save_opf(self.epub_path, self.opf_path, self.data)
