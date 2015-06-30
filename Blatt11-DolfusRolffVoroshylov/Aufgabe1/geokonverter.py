import argparse
import sys
from Geokonverter.Encoder.EncoderORCA import EncoderORCA
from Geokonverter.Encoder.EncoderTURBOMOLE import EncoderTURBOMOLE
from Geokonverter.Encoder.EncoderXYZ import EncoderXYZ
from Geokonverter.GeoConverter import GeoConverter


def main(files, batch, format, output=False):
    converter = GeoConverter([
        EncoderXYZ(),
        EncoderTURBOMOLE(),
        EncoderORCA(),
    ])

    buffer = sys.stdout
    if output is not False:
        buffer = open(output, 'w+')

    for num, line in enumerate(converter.process(files, batch), 0):
        buffer.write(("\n#----#\n" if num > 0 else "") + converter.encode(line, format))


if __name__ == '__main__':
    parser = argparse.ArgumentParser(prog='geokonverter.py',
                                     usage="%(prog) s -o <Ausgabeformat> " \
                                           "[Optionen] <Input-Datei(en)>")

    parser.add_argument('-o', '--format', default=None, action='store',
                        help="Die Angabe des Ausgabeformats " \
                             "(o = ORCA, t = TURBOMOLE, x = xyz-Format)" \
                             " ist obligatorisch")

    parser.add_argument('-f', '--file', default=False, action='store',
                        help="Angabe einer Ausgabedatei, deren Name" \
                             "angegeben werden muss,  andernfalls " \
                             "wird auf die Standardausgabe geschrieben")
    parser.add_argument('-b', '--batch', default=False, action='store_true',
                        help="Batch-Modus, es können beliebig viele " \
                             "Dateien angegeben werden, deren Formate" \
                             "alle in das Ausgabeformat konvertiert " \
                             "werden")

    parser.add_argument('input', nargs="*", action="store",
                        help='list of input files (blank separated) to process ')

    args = parser.parse_args()
    if args.format is None:
        exit(parser.print_help())

    main(args.input, args.batch, args.format, args.file)

