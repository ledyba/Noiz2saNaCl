var noiz2sa;
if(!noiz2sa) noiz2sa={};

noiz2sa.log = function(){
	var msg = "";
	for(var i=0;i<arguments.length; ++i){
		msg += arguments[i];
	}
	console.log(msg);
	$("#state").text(msg);
};

noiz2sa.moduleLoaded = function(){
	noiz2sa.log("Module loaded");
};

noiz2sa.handleMessage = function(msg){
	var payload = msg.data;
	noiz2sa.log(payload);
};

(function(){
	$(document).ready(function(){
		noiz2sa.log("Document loaded.");
		// モジュールのロード処理。これをつけないと、かなりの確率でロードに失敗してしまうみたい。
		// Googleのサンプルでもそうなっております
		var embed = $('<embed>');
		embed.attr('name','nacl_module');
		embed.attr('id', 'nes_nacl');
		embed.attr('width', '640');
		embed.attr('height', '480');
		embed.attr('type', 'application/x-nacl');
		embed.attr('src', 'noiz2sa.nmf');
		$('#nes_screen').append(embed);
		
		// モジュールがロードされた時の処理
		$("#nes_screen")[0].addEventListener('load', noiz2sa.moduleLoaded, true);
		$("#nes_screen")[0].addEventListener('message', noiz2sa.handleMessage, true);
		
	});
}());

