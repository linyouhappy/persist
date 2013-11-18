function ws(address, flags) {
	this.ws = null;
	this.st = null;
	this.id = null;
	
	var prefix = 'ws://';
	if (undefined === flags) {
		var address = prefix + address;
	}
	this.address = address;
}

ws.prototype.connect  = function() {
	this.ws = new WebSocket(this.address);
	
	this.ws.data = this;
	
	this.ws.onopen  = this._open;
	this.ws.onclose = this._close;
	
	this.ws.onerror = this._error;
	this.ws.onmessage = this._message;
}

ws.prototype.close = function() {
	console.log(this.ws);
	this.ws.close();
}

ws.prototype.send = function() {
	this.ws.send("123456");
}

ws.prototype.set      = function(object) {
	this.id = '#'+object.id;  //  信息输出ID
}

ws.prototype._open    = function(evt) {
	var id = this.data.id;
	$(id).html("连接成功"); 
}
ws.prototype._close   = function(evt) { }
ws.prototype._error   = function(evt) {
	var id = this.data.id;
	if (null != id) {
		$(id).html('Error Occured: ' + event.data); 
	} else {
		console.log('Error Occured: ' + event.data);
	}
}

ws.prototype._message = function(evt) { }