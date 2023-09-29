import argparse
from os import system
import subprocess


if __name__ == "__main__":

    parser = argparse.ArgumentParser(description='Convert text to html')
    
    parser.add_argument('filename')                                 # positional argument
    args = parser.parse_args()
    filename = args.filename



    f = open(filename, 'r');
    text = f.read()
    f.close();

    # " ' &  > < and spaces and tabs
    text = text.replace('"', "&#34;")
    text = text.replace("'", "&#39;")
    text = text.replace("&", "&#38;")
    text = text.replace("<", "&#60;")
    text = text.replace(">", "&#62;")
    text = text.replace(" ", "&nbsp;")
    text = text.replace("\t", "&emsp;")
    text = text.replace("\n", "<br>")

    new_filename = filename[:filename.find('.')] + ".html"

    subprocess.run(["rm", filename[:-2] + ".html"])
    subprocess.run(["touch", filename[:-2] + ".html"])

    s = '''<!DOCTYPE html>
<html>
<head>
<title>''' + filename[:-2] + '''</title>
<style>
html, body {height: 100%;margin: 0;}
.text_box {height:100%;width:100%;padding: 0px; white-space: nowrap; border:0px solid #000000;font:14px Georgia, Serif;background-color:#757575;}
</style>
</head>
<body>
<div class="text_box">
''' + text + '''
</div>
</body>
</html>'''

    f = open(filename[:-2] + ".html", 'w');
    f.write(s)
    f.close()