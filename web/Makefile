TARGETS=treemap.min.js treemap.css onefile.html node_info_consumer_html_include.c node_modules
all: $(TARGETS)

node_modules:
	npm install

clean:
	rm -rf $(TARGETS)

onefile.html: index.html treemap.css treemap.min.js
	/bin/cp -f index.html onefile.html
	perl -pe 's{<link.*?>}{`echo ""; echo "<style>";cat treemap.css; echo ""; echo "</style>"`}ge' -i onefile.html
	perl -pe 's{<script.*?treemap.*?>}{`echo ""; echo "<script>";cat treemap.min.js; echo ""`}ge' -i onefile.html

node_info_consumer_html_include.c: onefile.html
	cat onefile.html | perl -ape 's{\n}{}ms' | perl -ape 's{"\*\*\*".*}{}' | perl -ape 'BEGIN{print "const char node_info_consumer_html_prefix[] = {"};END{print "};\n"};s{(.)}{ord($$1).","}eg' | perl -ape 's{^(.*,)(.*?)$$}{$${1}0$${2}}' >node_info_consumer_html_include.c
	cat onefile.html | perl -ape 's{\n}{}ms' | perl -ape 's{.*"\*\*\*"}{}' | perl -ape 'BEGIN{print "const char node_info_consumer_html_suffix[] = {"};END{print "};\n"};s{(.)}{ord($$1).","}eg' | perl -ape 's{^(.*,)(.*?)$$}{$${1}0$${2}}' >>node_info_consumer_html_include.c
	/bin/cp -f node_info_consumer_html_include.c ../src/node_info_consumer_html_include.c

treemap.min.js: treemap.js node_modules
	cat treemap.js | npm run uglifyjs -- -c evaluate=false,inline=0 > treemap.min.js

treemap.css: treemap.styl node_modules
	npm run stylus -- -c treemap.styl

