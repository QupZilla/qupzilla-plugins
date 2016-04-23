%1

%2

function callReadability(){
	var loc = document.location;
	var uri = {
  		spec: loc.href,
  		host: loc.host,
  		prePath: loc.protocol + "//" + loc.host,
  		scheme: loc.protocol.substr(0, loc.protocol.indexOf(":")),
  		pathBase: loc.protocol + "//" + loc.host + loc.pathname.substr(0, loc.pathname.lastIndexOf("/") + 1)
	};
	var article = new Readability(uri, document).parse();
	renderPage(article);
}

function readabilityHead(article){
	var head = document.createElement('head');
	document.documentElement.appendChild(head);

	var title = document.createElement('title');
	title.innerHTML = article.title;
	head.appendChild(title);

    var css = document.createElement('style');
    css.innerHTML = '%3';
	head.appendChild(css);

   	var meta  = document.createElement('meta');
    meta.content = "text/html; charset=UTF-8";
    meta.httpEquiv = 'content-type';
    head.appendChild(meta);
    meta = document.createElement('meta');
    meta.name = 'viewport';
    meta.content = 'width=device-width, user-scalable=0';
    head.appendChild(meta);
}

function readablilityBody(article){
	var body = document.createElement('body');
	body.className = 'loaded sans-serif sepia';
	document.documentElement.appendChild(body);

	var container = document.createElement('div');
	container.className = 'container font-size5';
	container.id = 'container';
	body.appendChild(container);

	var header = document.createElement('div');
	header.className = 'header';
	header.id = 'reader-header';
	header.style.display = 'block';
	container.appendChild(header);

	var domain = document.createElement('a');
	domain.id = 'reader-domain';
	domain.className = 'domain';
	domain.href = article.uri.spec;
	domain.innerHTML = article.uri.host;
	header.appendChild(domain);

	var h1 = document.createElement('h1');
	h1.id = 'reader-title';
	h1.innerHTML = article.title;
	header.appendChild(h1);

	var credit = document.createElement('div');
	credit.id = 'reader-credits';
	credit.className = 'credits';
	credit.innerHTML = article.byline;
	header.appendChild(credit);

	var content = document.createElement('div');
	content.className = 'content';
	container.appendChild(content);

	var reader_content = document.createElement('div');
	reader_content.id = 'moz-reader-content';
	reader_content.style.display = 'block';
	reader_content.innerHTML = article.content;
	content.appendChild(reader_content);
}

function renderPage(article){
	var element = document.documentElement;
	if(element && !!article && !!article.content){
		element.removeChild(document.body);
		element.removeChild(document.head);

		readabilityHead(article);
		readablilityBody(article);

		readablilityToolbar();
	} else {
		alert("No content to transform.");
	}
}

callReadability();