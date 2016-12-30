/*!
 * Thumbnail helper for fancyBox
 * version: 1.0.2
 * @requires fancyBox v2.0 or later
 *
 * Usage: 
 *     $(".fancybox").fancybox({
 *         thumbs: {
 *             width : 50,
 *             height : 50
 *         }
 *     });
 * 
 * Options:
 *     width - thumbnail width
 *     height - thumbnail height
 *     source - function to obtain the URL of the thumbnail image
 *     position - 'top' or 'bottom'
 * 
 */
(function($){var F=$.fancybox;F.helpers.thumbs={wrap:null,list:null,width:0,source:function(el){var img=$(el).find('img');return img.length?img.attr('src'):el.href;},init:function(opts){var that=this,list,thumbWidth=opts.width||50,thumbHeight=opts.height||50,thumbSource=opts.source||this.source;list='';for(var n=0;n<F.group.length;n++){var styleClass=' ';if(n==0)styleClass=' class="base" ';list+='<li><a'+styleClass+'style="width:'+thumbWidth+'px;height:'+thumbHeight+'px;" href="javascript:jQuery.fancybox.jumpto('+n+');"></a></li>';}
this.wrap=$('<div id="fancybox-thumbs"></div>').addClass(opts.position||'bottom').appendTo('body');this.list=$('<ul>'+list+'</ul>').appendTo(this.wrap);$.each(F.group,function(i){$("<img />").load(function(){var width=this.width,height=this.height,widthRatio,heightRatio,parent;if(!that.list||!width||!height){return;}
widthRatio=width/thumbWidth;heightRatio=height/thumbHeight;parent=that.list.children().eq(i).find('a');if(widthRatio>=1&&heightRatio>=1){if(widthRatio>heightRatio){width=Math.floor(width/heightRatio);height=thumbHeight;}else{width=thumbWidth;height=Math.floor(height/widthRatio);}}
$(this).css({width:width,height:height,top:Math.floor(thumbHeight/2-height/2),left:Math.floor(thumbWidth/2-width/2)});parent.width(thumbWidth).height(thumbHeight);$(this).hide().appendTo(parent).fadeIn(300);}).attr('src',thumbSource(this));});this.width=this.list.children().eq(0).outerWidth();this.list.width(this.width*(F.group.length+1)).css('left',Math.floor(($(window).width()*0.5)-((this.width*F.group.length)*0.5)));},update:function(opts){},beforeLoad:function(opts){if(F.group.length<2){F.coming.helpers.thumbs=false;return;}
F.coming.margin[opts.position==='top'?0:2]=opts.height+30;},afterShow:function(opts){if(this.list){this.update(opts);}else{this.init(opts);}
this.list.children().removeClass('active').eq(F.current.index).addClass('active');},onUpdate:function(){this.update();},beforeClose:function(){if(this.wrap){this.wrap.remove();}
this.wrap=null;this.list=null;this.width=0;}}}(jQuery));