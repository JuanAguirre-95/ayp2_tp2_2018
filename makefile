nombre_tp = tp2

archivos = analog.c makefile

git_add:
	git add $(archivos)
git_commit: git_add
	@read -p "Mensaje de commit: " mensaje;
	git commit -m $mensaje
	
git_pull:
	git pull origin
	
git_push: git_commit
	git push origin https://github.com/Juan-Aguirre1/ayp2_tp2_2018.git
