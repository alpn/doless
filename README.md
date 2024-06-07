## doless

execute commands restrictively on OpenBSD

### install
```bash
git clone https://github.com/alpn/doless.git
cd doless
make && doas make install
```

### usage

```bash
$ doless -p "stdio rpath cpath wpath proc prot_exec tty" \
         -l -A "/home/a/.node_repl_history" /usr/local/bin/node
```

See man page for more information.
