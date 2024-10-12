This is a test container using kanidm to make sure libnss_igshim actually works.

While working on it, i usually have a shell with `./testcontainer/run.sh` running.

Then to test what your code does you can run `testrun` (make sure Kanidm has finished starting, it should say something like `Found kanidm_unixd, waiting for tasks`)

A correct response is as follows:
```
+ id doridian
uid=2006(doridian) gid=2006(doridian) groups=2006(doridian),4242(login-users),1001(share),4269(superadmins)
+ id root
uid=0(root) gid=0(root) groups=0(root),0(root),1(bin),2(daemon),3(sys),4(adm),6(disk),10(wheel),11(floppy),20(dialout),26(tape),27(video)
```
