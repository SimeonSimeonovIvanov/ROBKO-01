/*!

    Copyright (c) 2011 Peter van der Spek

    Permission is hereby granted, free of charge, to any person obtaining a copy
    of this software and associated documentation files (the "Software"), to deal
    in the Software without restriction, including without limitation the rights
    to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
    copies of the Software, and to permit persons to whom the Software is
    furnished to do so, subject to the following conditions:

    The above copyright notice and this permission notice shall be included in
    all copies or substantial portions of the Software.

    THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
    IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
    FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
    AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
    LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
    OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
    THE SOFTWARE.
    
 */
(function($){var liveUpdatingTargetSelectors={};var liveUpdaterIntervalId;var liveUpdaterRunning=false;var defaultSettings={ellipsis:'...',setTitle:'never',live:false};$.fn.ellipsis=function(selector,options){var subjectElements,settings;subjectElements=$(this);if(typeof selector!=='string'){options=selector;selector=undefined;}
settings=$.extend({},defaultSettings,options);settings.selector=selector;subjectElements.each(function(){var elem=$(this);ellipsisOnElement(elem,settings);});if(settings.live){addToLiveUpdater(subjectElements.selector,settings);}else{removeFromLiveUpdater(subjectElements.selector);}
return this;};function ellipsisOnElement(containerElement,settings){var containerData=containerElement.data('jqae');if(!containerData)containerData={};var wrapperElement=containerData.wrapperElement;if(!wrapperElement){wrapperElement=containerElement.wrapInner('<div/>').find('>div');wrapperElement.css({margin:0,padding:0,border:0});}
var wrapperElementData=wrapperElement.data('jqae');if(!wrapperElementData)wrapperElementData={};var wrapperOriginalContent=wrapperElementData.originalContent;if(wrapperOriginalContent){wrapperElement=wrapperElementData.originalContent.clone(true).data('jqae',{originalContent:wrapperOriginalContent}).replaceAll(wrapperElement);}else{wrapperElement.data('jqae',{originalContent:wrapperElement.clone(true)});}
containerElement.data('jqae',{wrapperElement:wrapperElement,containerWidth:containerElement.width(),containerHeight:containerElement.height()});var containerElementHeight=containerElement.height();var wrapperOffset=(parseInt(containerElement.css('padding-top'),10)||0)+(parseInt(containerElement.css('border-top-width'),10)||0)-(wrapperElement.offset().top-containerElement.offset().top);var deferAppendEllipsis=false;var selectedElements=wrapperElement;if(settings.selector)selectedElements=$(wrapperElement.find(settings.selector).get().reverse());selectedElements.each(function(){var selectedElement=$(this),originalText=selectedElement.text(),ellipsisApplied=false;if(wrapperElement.innerHeight()-selectedElement.innerHeight()>containerElementHeight+wrapperOffset){selectedElement.remove();}else{removeLastEmptyElements(selectedElement);if(selectedElement.contents().length){if(deferAppendEllipsis){getLastTextNode(selectedElement).get(0).nodeValue+=settings.ellipsis;deferAppendEllipsis=false;}
while(wrapperElement.innerHeight()>containerElementHeight+wrapperOffset){ellipsisApplied=ellipsisOnLastTextNode(selectedElement);if(ellipsisApplied){removeLastEmptyElements(selectedElement);if(selectedElement.contents().length){getLastTextNode(selectedElement).get(0).nodeValue+=settings.ellipsis;}else{deferAppendEllipsis=true;selectedElement.remove();break;}}else{deferAppendEllipsis=true;selectedElement.remove();break;}}
if(((settings.setTitle=='onEllipsis')&&ellipsisApplied)||(settings.setTitle=='always')){selectedElement.attr('title',originalText);}else if(settings.setTitle!='never'){selectedElement.removeAttr('title');}}}});}
function ellipsisOnLastTextNode(element){var lastTextNode=getLastTextNode(element);if(lastTextNode.length){var text=lastTextNode.get(0).nodeValue;var pos=text.lastIndexOf(' ');if(pos>-1){text=$.trim(text.substring(0,pos));lastTextNode.get(0).nodeValue=text;}else{lastTextNode.get(0).nodeValue='';}
return true;}
return false;}
function getLastTextNode(element){if(element.contents().length){var contents=element.contents();var lastNode=contents.eq(contents.length-1);if(lastNode.filter(textNodeFilter).length){return lastNode;}else{return getLastTextNode(lastNode);}}else{element.append('');var contents=element.contents();return contents.eq(contents.length-1);}}
function removeLastEmptyElements(element){if(element.contents().length){var contents=element.contents();var lastNode=contents.eq(contents.length-1);if(lastNode.filter(textNodeFilter).length){var text=lastNode.get(0).nodeValue;text=$.trim(text);if(text==''){lastNode.remove();return true;}else{return false;}}else{while(removeLastEmptyElements(lastNode)){}
if(lastNode.contents().length){return false;}else{lastNode.remove();return true;}}}
return false;}
function textNodeFilter(){return this.nodeType===3;}
function addToLiveUpdater(targetSelector,settings){liveUpdatingTargetSelectors[targetSelector]=settings;if(!liveUpdaterIntervalId){liveUpdaterIntervalId=window.setInterval(function(){doLiveUpdater();},200);}}
function removeFromLiveUpdater(targetSelector){if(liveUpdatingTargetSelectors[targetSelector]){delete liveUpdatingTargetSelectors[targetSelector];if(!liveUpdatingTargetSelectors.length){if(liveUpdaterIntervalId){window.clearInterval(liveUpdaterIntervalId);liveUpdaterIntervalId=undefined;}}}};function doLiveUpdater(){if(!liveUpdaterRunning){liveUpdaterRunning=true;for(var targetSelector in liveUpdatingTargetSelectors){$(targetSelector).each(function(){var containerElement,containerData;containerElement=$(this);containerData=containerElement.data('jqae');if((containerData.containerWidth!=containerElement.width())||(containerData.containerHeight!=containerElement.height())){ellipsisOnElement(containerElement,liveUpdatingTargetSelectors[targetSelector]);}});}
liveUpdaterRunning=false;}};})(jQuery);