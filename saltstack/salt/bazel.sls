bazel-repo:
  pkgrepo.managed:
    - name: deb [arch=amd64] http://storage.googleapis.com/bazel-apt stable jdk1.8
    - key_url: https://bazel.build/bazel-release.pub.gpg
    - require_in: bazel

bazel:
  pkg.latest
