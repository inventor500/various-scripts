# passwd-audit

This scans through `/etc/passwd` to check for passwords stored in that file. It is recommended that passwords be stored in `/etc/shadow` instead.

Just because a password is marked as being stored in `/etc/shadow` does not mean that password login is actually enabled, since `/etc/shadow` can also disable password login for an account.

## Usage

```shell
$ ./passwd-audit
# Username            Password
# root                In /etc/shadow
# ...
```

## Q&A

- Why C++?  
It has been a while since I wrote anything in C++.
- Why not just use `grep` if you actually need to do this?  
This short program is mostly just to reinforce my own learning, and not for any real use.
