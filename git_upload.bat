@echo off
echo git_upload
git pull
git add .
git commit -m "none"
git push
pause