nombre_tp = tp2

archivos = analog.c makefile
mensaje = 
git_add:
	git add $(archivos)
git_commit: git_add
	@echo "Mensaje de commit: "
	@read mensaje; \
	git commit -m $$mensaje
	
git_pull:
	git pull origin
	
git_push: git_commit
	git push origin master
