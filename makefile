nombre_tp = tp2

archivos = analog.c makefile
mensaje = 
git_add:
	git add $(archivos)
git_commit: git_add
	@read -p "Enter file name: " FILENAME; \
    git commit -m "$$FILENAME"
	
git_pull:
	git pull origin
	
git_push: git_commit
	git push origin master
