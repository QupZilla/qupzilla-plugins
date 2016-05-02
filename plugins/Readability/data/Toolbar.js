/* ============================================================
* QupZilla - WebKit based browser
* Copyright (C) 2016  Jaroslav Bambas <jar.bambas@gmail.com>
*
* This program is free software: you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation, either version 3 of the License, or
* (at your option) any later version.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with this program.  If not, see <http://www.gnu.org/licenses/>.
* ============================================================ */

function readablilityToolbar(){
    renderToolbar();
    addListeners();
}

function renderToolbar(){
    var toolbar = document.createElement('div');
    document.getElementById('container').appendChild(toolbar);

    toolbar.innerHTML = '<ul visible="true" id="reader-toolbar" class="toolbar"><li><button id="close-button" class="button close-button"></button></li><ul id="style-dropdown" class="dropdown"><li><button id="style-button" class="dropdown-toggle button style-button"></button></li><li style="top: 48px;" id="reader-popup" class="dropdown-popup"><div id="font-type-buttons"><button id="sans-serif-button" class="sans-serif-button selected"><div class="name">Aa</div><div class="description">Sans-serif</div></button><button id="serif-button" class="serif-button"><div class="name">Aa</div><div class="description">Serif</div></button></div><hr><div id="font-size-buttons"><button id="font-size-minus" class="minus-button"></button><button id="font-size-plus" class="plus-button"></button></div><hr><div id="color-scheme-buttons"><button id="light-button" class="light-button"><div class="name">Light</div></button><button id="dark-button" class="dark-button"><div class="name">Dark</div></button><button id="sepia-button" class="sepia-button selected"><div class="name">Sepia</div></button></div><div class="dropdown-arrow"></div></li></ul></ul>';
}

function addListeners(){
    document.getElementById('close-button').addEventListener("click", closeReadability);
    document.getElementById('style-button').addEventListener("click", toolbarService);

    var lightButton = document.getElementById('light-button');
    lightButton.addEventListener("click", function(){switchToLight(lightButton);});
    var sepiaButton = document.getElementById('sepia-button');
    sepiaButton.addEventListener("click", function(){switchToSepia(sepiaButton);});
    var darkButton = document.getElementById('dark-button');
    darkButton.addEventListener("click", function(){switchToDark(darkButton);});

    var serifButton = document.getElementById('serif-button');
    serifButton.addEventListener("click", function(){switchToSerif(serifButton);});
    var sansSerifButton = document.getElementById('sans-serif-button');
    sansSerifButton.addEventListener("click", function(){switchToSansSerif(sansSerifButton);});

    var fontSizePlusButton = document.getElementById('font-size-plus');
    fontSizePlusButton.addEventListener("click", fontSizePlus);
    var fontSizeMinusButton = document.getElementById('font-size-minus');
    fontSizeMinusButton.addEventListener("click", fontSizeMinus);
}

function closeReadability(){
    location.reload();
}

function toolbarService(){
    var element = document.getElementById('style-dropdown');

    if(element.classList.contains('open')){
        element.classList.remove('open');
    } else {
        element.classList.add('open');

    }
}

function switchToLight(item){
    document.body.classList.remove('sepia', 'dark');
    document.body.classList.add('light');

    var items = document.getElementById('color-scheme-buttons').children;
    for (var j = items.length - 1; j >= 0; j--) {
        items[j].classList.remove("selected");
    }
    item.classList.add('selected');
}

function switchToSepia(item){
    document.body.classList.remove('light', 'dark');
    document.body.classList.add('sepia');

    var items = document.getElementById('color-scheme-buttons').children;
    for (var j = items.length - 1; j >= 0; j--) {
        items[j].classList.remove("selected");
    }
    item.classList.add('selected');
}

function switchToDark(item){
    document.body.classList.remove('sepia', 'light');
    document.body.classList.add('dark');

    var items = document.getElementById('color-scheme-buttons').children;
    for (var j = items.length - 1; j >= 0; j--) {
        items[j].classList.remove("selected");
    }
    item.classList.add('selected');
}

function switchToSerif(item){
    document.body.classList.remove('sans-serif');
    document.body.classList.add('serif');
    document.getElementById('sans-serif-button').classList.remove('selected');
    item.classList.add('selected');
}

function switchToSansSerif(item){
    document.body.classList.remove('serif');
    document.body.classList.add('sans-serif');
    document.getElementById('serif-button').classList.remove('selected');
    item.classList.add('selected');
}

function fontSizePlus(){
    var container = document.getElementById('container');
    var incrementedSize = 0;
    for (var i = 0; i < container.classList.length; i++){
        if(container.classList[i].indexOf('font-size') > -1){
            var size = container.classList[i].substr(-1, 1);
            if(parseInt(size) < 9){
                incrementedSize = parseInt(size) + 1;
            } else return;
            container.classList.remove(container.classList[i]);
            container.classList.add('font-size' + incrementedSize);
            return;
        }
    }
}

function fontSizeMinus(){
    var container = document.getElementById('container');
    var decrementedSize = 0;
    for (var i = 0; i < container.classList.length; i++){
        if(container.classList[i].indexOf('font-size') > -1){
            var size = container.classList[i].substr(-1, 1);
            if(parseInt(size) > 1){
                incrementedSize = parseInt(size) - 1;
            } else return;
            container.classList.remove(container.classList[i]);
            container.classList.add('font-size' + incrementedSize);
            return;
        }
    }
}