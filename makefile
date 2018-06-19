nombre_tp = tp2

archivos = analog.c makefile

git_add:
	git add $(archivos)
git_commit: git_add
	@read -p "Mensaje del commit: " MENSAJE; \
    git commit -m "$$MENSAJE"
	
git_pull:
	git pull origin
	
git_push:
	git push origin master
