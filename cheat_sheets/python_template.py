import argparse
import os.system



if __name__ == "__main__":

    parser = argparse.ArgumentParser(description='Convert text to html')
    
    parser.add_argument('filename')                                 # positional argument
    parser.add_argument('-c', '--count')                            # option that takes a value
    parser.add_argument('-v', '--verbose', action='store_true')     # on/off flag
    args = parser.parse_args()
