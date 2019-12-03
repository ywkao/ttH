from subprocess import call
for i in range(10):
    cmd = ""
    for j in range(10):
        cmd += "source combineCmd_sig_"+str(i+j*10)+".sh"
        if j < 9: cmd += ";"
    cmd += "&"
    call("echo \"" + cmd + "\" > run_" + str(i)+ ".sh",shell=True)
