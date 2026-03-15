import zipfile
import xml.etree.ElementTree as ET
import sys

def get_opf_path(epub_path: str) -> str:
    with zipfile.ZipFile(epub_path, 'r') as epub:
        with epub.open("META-INF/container.xml") as f:
            tree = ET.parse(f)

    namespace = {"ns": "urn:oasis:names:tc:opendocument:xmlns:container"}

    root = tree.getroot()
    rootfile = root.find(".//ns:rootfile", namespace)
    if rootfile is None:
        raise ValueError("No <rootfile> element found in container.xml")

    full_path = rootfile.get("full-path")
    if not full_path:
        raise ValueError("<rootfile> element has no 'full-path' attribute")

    return full_path


if __name__ == "__main__":
    if len(sys.argv) != 2:
        print("usage: container.py <path_to_your_file.epub>", file=sys.stderr)
        sys.exit(2)
    opf_path = get_opf_path(sys.argv[1])
    print(opf_path)
