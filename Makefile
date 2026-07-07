.PHONY: all clean function

fclean:
	git submodule foreach '$(MAKE) fclean'
