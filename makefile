nombre_tp = tp2

archivos = analog.c makefile
mensaje = 
git_add:
	git add $(archivos)
git_commit: git_add
	@read -p "Mensaje del commit: " MENSAJE; \
    git commit -m "$$MENSAJE"
	
git_pull:
	git pull origin
	
git_push: git_commit
	git push origin master