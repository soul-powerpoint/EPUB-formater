from src.epub_io.opf import OPFData, ManifestItem
import zipfile
import os

def set_language(data: OPFData, language: str) -> OPFData:
    if not language or not language.strip():
        raise ValueError("Language code cannot be empty")

    data.language = language.strip().lower()
    return data


def set_cover(epub_path: str, new_image_path: str, cover_item: ManifestItem) -> None:
    cover_zip_path = cover_item.href
    tmp_path = epub_path + ".tmp"

    with open(new_image_path, "rb") as img_f:
        new_image_bytes = img_f.read()

    with zipfile.ZipFile(epub_path, "r") as src, \
         zipfile.ZipFile(tmp_path, "w", zipfile.ZIP_DEFLATED) as dst:

        for item in src.infolist():
            if item.filename.endswith(cover_zip_path):
                dst.writestr(item, new_image_bytes)
            else:
                dst.writestr(item, src.read(item.filename))

    os.replace(tmp_path, epub_path)
