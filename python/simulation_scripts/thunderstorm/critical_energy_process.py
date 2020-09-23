
import argparse

from phd.thunderstorm.critical_energy import calculate_secondary_production_rate
import numpy as np

def process_secondary_rate(args):
    result = []
    for file in args.files:
        data = calculate_secondary_production_rate(file)
        result.append(data)
    if len(result) > 1:
        data = np.vstack(result)
    else:
        data = result[0]
    np.save(args.output, data)
    return 0

def create_parser():
    parser = argparse.ArgumentParser(description='Process critical energy.')
    parser.add_argument('files', metavar='FILE', type=str, nargs='+',
                        help='an files for processing')
    parser.add_argument('--secondary-rate', action='store_true',
                        help='calculate rate of secondary production for all parameters')
    parser.add_argument("--output", "-o", action="store", help="Output file name", default="critical_energy")
    return parser


def main():
    args = create_parser().parse_args()
    if args.seconary_rate:
        process_secondary_rate(args)
    return 0

if __name__ == '__main__':
    main()