
import argparse
import numpy as np
from phd.thunderstorm.minimal_field import plot_minimal_field_production, find_minimal_field


def plot_seconadry(args):
    for file in args.files:
        plot_minimal_field_production(file, output=args.output)
    return 0

def process_minimal_field(args):
    result = []
    for file in args.files:
        data = find_minimal_field(file)
        result.append(data)
    if len(result) > 1:
        data = np.hstack(result)
    else:
        data = result[0]
    np.save(args.output, data)
    return 0

def create_parser():
    parser = argparse.ArgumentParser(description='Process critical energy.')
    parser.add_argument('files', metavar='FILE', type=str, nargs='+',
                        help='an files for processing')
    # parser.add_argument('--secondary-rate', action='store',
    #                     help='calculate rate of secondary production for all parameters', default=None)
    parser.add_argument('--production-plot', action='store_true',
                        help='Plot secondary production for all parameters')
    parser.add_argument("--output", "-o", action="store", help="Output file name", default="minimal_field")
    return parser


def main():
    args = create_parser().parse_args()
    if args.production_plot:
        plot_seconadry(args)
    return 0

if __name__ == '__main__':
    main()