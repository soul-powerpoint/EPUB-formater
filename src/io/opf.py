import zipfile
import xml.etree.ElementTree as ET
from dataclasses import dataclass, field
from container import get_opf_path
import sys

OPF_NS  = "http://www.idpf.org/2007/opf"
DC_NS   = "http://purl.org/dc/elements/1.1/"

NS = {
    "opf": OPF_NS,
    "dc":  DC_NS,
}


@dataclass
class ManifestItem:
    id:         str
    href:       str
    media_type: str
    properties: str = ""


@dataclass
class SpineItem:
    idref:  str
    linear: bool = True


@dataclass
class OPFData:
    title:      str = ""
    author:     str = ""
    language:   str = ""
    publisher:  str = ""
    identifier: str = ""

    manifest: list[ManifestItem] = field(default_factory=list)
    spine:    list[SpineItem]    = field(default_factory=list)


def parse_opf(epub_path: str, opf_path: str) -> OPFData:
    with zipfile.ZipFile(epub_path, "r") as epub:
        with epub.open(opf_path) as f:
            tree = ET.parse(f)

    root = tree.getroot()
    data = OPFData()

    _parse_metadata(root, data)
    _parse_manifest(root, data)
    _parse_spine(root, data)

    return data


def _parse_metadata(root: ET.Element, data: OPFData) -> None:
    meta = root.find("opf:metadata", NS)
    if meta is None:
        return

    def text(tag: str) -> str:
        element = meta.find(tag, NS)
        return element.text.strip() if element is not None and element.text else ""

    data.title      = text("dc:title")
    data.author     = text("dc:creator")
    data.language   = text("dc:language")
    data.publisher  = text("dc:publisher")
    data.identifier = text("dc:identifier")


def _parse_manifest(root: ET.Element, data: OPFData) -> None:
    manifest = root.find("opf:manifest", NS)
    if manifest is None:
        return

    for item in manifest.findall("opf:item", NS):
        data.manifest.append(ManifestItem(
            id         = item.get("id", ""),
            href       = item.get("href", ""),
            media_type = item.get("media-type", ""),
            properties = item.get("properties", ""),
        ))


def _parse_spine(root: ET.Element, data: OPFData) -> None:
    spine = root.find("opf:spine", NS)
    if spine is None:
        return

    for itemref in spine.findall("opf:itemref", NS):
        linear_attr = itemref.get("linear", "yes")
        data.spine.append(SpineItem(
            idref  = itemref.get("idref", ""),
            linear = linear_attr.lower() != "no",
        ))


if __name__ == "__main__":
    if len(sys.argv) != 2:
        print("usage: opf.py <path_to_your_file.epub>", file = sys.stderr)
        sys.exit(2)

    opf_path = get_opf_path(sys.argv[1])
    data = parse_opf(sys.argv[1], opf_path)
    print(data)
