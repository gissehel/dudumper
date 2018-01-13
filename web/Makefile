
all: index.html treemap.js treemap.css jquery.js onefile.html node_parser_html_inlude.c



clean:
	rm -rf index.html treemap.js treemap.css jquery.js onefile.html node_parser_html_inlude.c

onefile.html: index.html treemap.css treemap.js jquery.js
	cp index.html onefile.html
	perl -pe 's{<link.*?>}{`echo ""; echo "<style>";cat treemap.css; echo ""; echo "</style>"`}ge' -i onefile.html
	perl -pe 's{<script.*?jquery.*?>}{`echo ""; echo "<script>";cat jquery.js; echo ""`}ge' -i onefile.html
	perl -pe 's{<script.*?treemap.*?>}{`echo ""; echo "<script>";cat treemap.js; echo ""`}ge' -i onefile.html

node_parser_html_inlude.c: onefile.html
	cat onefile.html | perl -ape 's{\n}{}ms' | perl -ape 's{"\*\*\*".*}{}' | perl -ape 'BEGIN{print "const char node_parser_html_prefix[] = {"};END{print "};\n"};s{(.)}{ord($$1).","}eg' | perl -ape 's{^(.*,)(.*?)$$}{$${1}0$${2}}' >node_parser_html_inlude.c
	cat onefile.html | perl -ape 's{\n}{}ms' | perl -ape 's{.*"\*\*\*"}{}' | perl -ape 'BEGIN{print "const char node_parser_html_suffix[] = {"};END{print "};\n"};s{(.)}{ord($$1).","}eg' | perl -ape 's{^(.*,)(.*?)$$}{$${1}0$${2}}' >>node_parser_html_inlude.c
	cp -f node_parser_html_inlude.c ../src/node_parser_html_inlude.c

index.html: index.jade
	pug index.jade

treemap.js: treemap.coffee
	coffee -c -b --no-header -p treemap.coffee | uglifyjs -c > treemap.js

treemap.css: treemap.styl
	stylus -c treemap.styl

jquery.js:
	curl http://code.jquery.com/jquery-3.1.1.min.js > jquery.js

