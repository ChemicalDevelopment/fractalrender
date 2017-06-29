
/*

ChemicalDevelopment 2017

*/




var mand_conf_default_dict = {
  cX: "0.0",
  cY: "0.0"
};

var mand_conf_dict = mand_conf_default_dict;

var mand_conf_dict_last;

$(window).on('load', function () {
    mand_init();
});

function mand_init() {
    mand_conf_dict = mand_conf_default_dict;

    var serialized = window.location.hash.replace("#/viewer?", "");
    serialized = serialized.replace("#/viewer", "");

    var _keyvals = serialized.split("&");
    var keyvals = [];

    for (var i = 0; i < _keyvals.length; ++i) {
        keyvals.push(_keyvals[i].split("="));
    }

    console.log("keyvals: " + keyvals);

    for (var i = 0; i < keyvals.length; ++i) {
        if (keyvals[i][1] != "") {
            mand_conf_dict[keyvals[i][0]] = keyvals[i][1];
        }
    }

    setTimeout(function (){
        $('#mand_conf').populate(mand_conf_dict);
    }, 0);

}

function mand_update() {
    var srl_form = $('#mand_conf').serialize();
    console.log("Updating");
    console.log("Serialized config: " + srl_form);
    window.location.hash = '#/viewer?' + srl_form;
    console.log("New hash: " + window.location.hash)
    //window.location.hash = "testing";
}
