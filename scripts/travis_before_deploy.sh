#!/bin/bash -x
set -ex

openssl aes-256-cbc -K $encrypted_8ebb1ef83f64_key -iv $encrypted_8ebb1ef83f64_iv -in $TRAVIS_BUILD_DIR/github_deploy_key.enc -out github_deploy_key -d
chmod 600 github_deploy_key
eval $(ssh-agent -s)
ssh-add github_deploy_key

git tag -f latest-passing-build -a -m "Generated tag from TravisCI for build $TRAVIS_BUILD_NUMBER"
git push -f git@github.com:cpputest/cpputest.git --tags
