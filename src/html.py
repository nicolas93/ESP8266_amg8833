import sys, os

if(len(sys.argv)>1):
    file = sys.argv[1]
    os.system("html-minifier --collapse-whitespace --minify-js 1 %s -o new.html" % file)
    f = open("new.html")
    html = f.read()
    re = ""
    for c in html:
        if(c == "\""):
            re = re + "\\\""
        else:
            re = re + c
    print re
    o = open("html.h", "w")
    o.write("#define INDEX " + re)
else:
    print "No file."
