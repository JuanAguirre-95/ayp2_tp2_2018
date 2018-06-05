nombre_tp = tp2


git_add:
	git add . 
git_commit: git_add
	@read -p "Mensaje de commit: " mensaje;
	git commit -m $mensaje
git_pull:
	git pull origin
git_push: git_commit
	git push origin master
