%1



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

function renderPage(article){
	var element=document.documentElement;
	if(element){
		element.removeChild(document.body);
		element.removeChild(document.head);

		var head  = document.createElement('head');
		var css   = document.createElement('link');
		var meta  = document.createElement('meta');
		var title = document.createElement('title');
		title.innerHTML=article.title;
		document.documentElement.appendChild(head);
		css.rel  = 'stylesheet';
    	css.type = 'text/css';
    	css.href = 'qrc:/readability/data/style.css';
    	meta.content = "text/html; charset=UTF-8";
    	meta.httpEquiv = 'content-type';
    	head.appendChild(meta);
    	meta = document.createElement('meta');
    	meta.name = 'viewport';
    	meta.content = 'width=device-width, user-scalable=0';
    	head.appendChild(meta);
		head.appendChild(css);
		head.appendChild(title);


		var body = document.createElement('body');
		body.className = 'loaded sans-serif sepia';
		var container = document.createElement('div');
		container.className = 'container font-size5';
		container.id = 'container';

		var header = document.createElement('div');
		header.className = 'header';
		header.id = 'reader-header';
		header.style.display = 'block';
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

		container.appendChild(header);


		var content = document.createElement('div');
		content.className = 'content';
		var reader_content = document.createElement('div');
		reader_content.id = 'moz-reader-content';
		reader_content.style.display = 'block';
		reader_content.innerHTML = article.content;
		content.appendChild(reader_content);

		container.appendChild(content);
		body.appendChild(container);
		



		document.documentElement.appendChild(body);
		console.log(article.excerpt);
	}
}

callReadability();