ensure latest llvm toolchain installed:
  pkg.latest:
    - pkgs:
      - clang
      - lldb

update-alternatives --set cc `which clang`:
  cmd.run

update-alternatives --set c++ `which clang++`:
  cmd.run
