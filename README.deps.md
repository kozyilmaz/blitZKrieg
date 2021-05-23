# blitZKrieg
ZK-based Attack Resistant Federated Machine Learning

## Dependencies
All dependencies are configured as subtrees for upgradability. Below are the exact commands for update.

### libmakefile
```
# add
$ git remote add libmakefile https://github.com/alperakcan/libmakefile.git
$ git subtree add --prefix=deps/libmakefile/ --squash libmakefile master
# check
$ git fetch https://github.com/alperakcan/libmakefile.git master
$ ./scripts/git-subtree-check.sh -r deps/libmakefile
# sync
$ git remote add libmakefile-remote https://github.com/alperakcan/libmakefile.git
$ git subtree pull --prefix=deps/libmakefile/ --squash libmakefile-remote master
```

### r128
```
# add
$ git remote add r128 https://github.com/fahickman/r128.git
$ git subtree add --prefix=deps/r128/ --squash r128 main
# check
$ git fetch https://github.com/fahickman/r128.git main
$ ./scripts/git-subtree-check.sh -r deps/r128
# sync
$ git remote add r128-remote https://github.com/fahickman/r128.git
$ git subtree pull --prefix=deps/r128/ --squash r128-remote main
```

### abdk-libraries-solidity
```
# add
$ git remote add abdk-libraries-solidity https://github.com/abdk-consulting/abdk-libraries-solidity.git
$ git subtree add --prefix=deps/abdk-libraries-solidity/ --squash abdk-libraries-solidity master
# check
$ git fetch https://github.com/abdk-consulting/abdk-libraries-solidity.git master
$ ./scripts/git-subtree-check.sh -r deps/abdk-libraries-solidity
# sync
$ git remote add abdk-libraries-solidity-remote https://github.com/abdk-consulting/abdk-libraries-solidity.git
$ git subtree pull --prefix=deps/abdk-libraries-solidity/ --squash abdk-libraries-solidity-remote master
```
