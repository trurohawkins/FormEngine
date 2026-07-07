.PHONY: all clean function

fclean:
	git submodule foreach '$(MAKE) fclean'

checkout:
	git submodule foreach 'git checkout main'
