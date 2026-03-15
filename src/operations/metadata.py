from epub_io.opf import OPFData

def set_language(data: OPFData, language: str) -> OPFData:
    if not language or not language.strip():
        raise ValueError("Language code cannot be empty")

    data.language = language.strip().lower()
    return data
