function ws(address, flags) {
	this.ws = null;
	this.st = -1;  //  -1  0  connecting  1 open  2 closing  3 closed
	this.id = null;
	
	var prefix = 'ws://';
	if (undefined === flags) {
		var address = prefix + address;
	}
	this.address = address;
	
	this.ws = this;
}

ws.prototype.connect  = function() {
	this.ws = new WebSocket(this.address);

	this.ws._open    = null;
	this.ws._close   = null;
	this.ws._error   = null;
	this.ws._message = null;
	
	//  扩展数据
	this.ws.data = this;
	
	//  注册事件函数
	this.ws.onopen  = this.onopen;
	this.ws.onclose = this.onclose;
//	this.ws.onerror = this.onerror;
	this.ws.onmessage = this.onmessage;
	
}

ws.prototype.close = function() {
	this.ws.close();
}

ws.prototype.send = function(val) {
	var id = this.id;
	var st = this.st;
	
	switch(st) {
	case -1:
	case 0:
		$(id).html("SOCKET未连接");
		break;
	case 1:
		$(id).html("发送数据:(" + val + ')');
		this.ws.send(val);
		break;
	}
}

ws.prototype.set      = function(object) {
	if (object.id) {
		this.id = '#'+object.id;  //  信息输出ID
	}
	
	if (object.open) {
		this._open = object.open
	}
	
	if (object.close) {
		this._close = object.close
	}
}

ws.prototype.onopen    = function(evt) {
	this.data.st = this.readyState;
	var id = this.data.id;
	$(id).html("连接成功"); 
	if (this.data._open) {
		this.data._open();
	}
}
ws.prototype.onclose   = function(evt) {
	var id = this.data.id;
	var st = this.data.st;
	console.log(st);
	switch(st) {
	case -1:
		$(id).html("连接失败");
		break;
	case 1:	// open
		$(id).html("关闭成功"); 
		break;
	}
	if (this.data._close) {
		this.data._close();
	}
}
//ws.prototype.onerror   = function(evt) {
//	var id = this.data.id;
//	if (null != id) {
//		$(id).html('Error Occured: ' + event.data); 
//	} else {
//		console.log('Error Occured: ' + event.data);
//	}
//}

ws.prototype.onmessage = function(evt) { }
