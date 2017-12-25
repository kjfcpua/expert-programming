## git-in-action

### command

#### config
```shell
git config --global user.email ${email}
git config --global user.name ${name}
git config user.email ${email}
git config user.name ${name}
```

#### list change
```shell
git status
```

#### commit
```shell
git commit -s -m "add git-in-action.md" # commit with signoff
git commit --amend -s --no-edit # signoff last commit
```

#### rebase
```shell
git rebase -i origin/master
```

#### reset
```shell
git clean -df # delete untracked files
git reset --hard # revert to prev version
git reset --hard origin/master # reset to server version 
```

#### other
```shell
git log
git gc --prune=now
git count-objects -v
```

### flows

#### commit to master
```shell
git branch -a
git checkout -b ${branch}
git commit -a -m "Fixes ${issue} ${msg}"
git commit --amend
git review -t ${topic}
```

#### commit to gerrit
```shell
git branch -a
git checkout -b brch remotes/origin/${branch}
git review -d ${change-id}
git cherry-pick ${commit}
git review -t ${topic} ${branch}
```
