.PHONY: clean update commit make
make: 
	echo "nothing"
update:
	git remote update
commit: clean
	git add -A
	git commit
	git push origin master
clean:
	find . -name "*.o" | xargs rm -f
