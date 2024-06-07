## doless

execute commands restrictively on OpenBSD

### Build / Install
```bash
git clone https://github.com/alpn/doless.git
cd doless
make && doas make install
```

### Usage

```bash
$ doless -p "stdio rpath cpath wpath proc prot_exec tty" \\
         -l -A "/home/a/.node_repl_history" /usr/local/bin/node
```

See man page for more information.
