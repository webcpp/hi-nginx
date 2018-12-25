var echo=function(){
    var data='hello, '
    this.concat=function(text){
        return data+text
    }
    this.set=function(text){
        data=text
    }
}
module.exports=echo