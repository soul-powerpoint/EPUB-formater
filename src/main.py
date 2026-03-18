import argparse
import sys
from .formator import EpubFormator

def command_info(args):
    fmt = EpubFormator(args.epub)
    print(f"Title: \t\t {fmt.get_title()}")
    print(f"Author: \t {fmt.get_author()}")
    print(f"Language: \t {fmt.get_language()}")
    print(f"Cover ID: \t {fmt.get_cover()}")


def command_set_language(args):
    fmt = EpubFormator(args.epub)
    fmt.set_language(args.language)
    print(f"Language set to {args.language}")


def command_set_cover(args):
    fmt = EpubFormator(args.epub)
    fmt.set_cover(args.image)
    print(f"Cover replaced with {args.image}")


def build_parser() -> argparse.ArgumentParser:
    parser = argparse.ArgumentParser(
        prog="epub-formater",
        description="A CLI tool for editing EPUB metadata",
    )
    parser.add_argument("epub", help="Path to the EPUB file")

    group = parser.add_mutually_exclusive_group(required=True)
    group.add_argument("-info", action="store_true", help="Show metadata")
    group.add_argument("-set-language", dest="language", help="BCP-47 language code")
    group.add_argument("-set-cover", dest="cover", help="Path to new cover image")

    return parser


def main():
    parser = build_parser()
    args = parser.parse_args()

    try:
        fmt = EpubFormator(args.epub)
        if args.info:
            print(f"Title:    {fmt.get_title()}")
            print(f"Author:   {fmt.get_author()}")
            print(f"Language: {fmt.get_language()}")
            print(f"Cover ID: {fmt.get_cover()}")
        elif args.language:
            fmt.set_language(args.language)
            print(f"Language set to '{args.language}'")
        elif args.cover:
            fmt.set_cover(args.cover)
            print(f"Cover replaced with '{args.cover}'")
    except Exception as e:
        print(f"Error: {e}", file=sys.stderr)
        sys.exit(1)


if __name__ == "__main__":
    main()
