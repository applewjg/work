for atrr in [0,1,4,5,7]:
    for fore in [30,31,32,33,34,35,36,37]:
        for back in [40,41,42,43,44,45,46,47]:
            color = "\x1B[%d;%d;%dm" % (atrr,fore,back)
            print "%s \\x1B[%d;%d;%d]blabla\\x1B[0m \x1B[0m" % (color,atrr,fore,back),
        print ""
