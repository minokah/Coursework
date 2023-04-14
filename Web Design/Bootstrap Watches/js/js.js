// scrollfire and parallax
var scrollfire = document.getElementById("devices");
var parallax = document.getElementById("parallax");
function scroll() {
    console.log(window.scrollY);
    if (window.scrollY > 3300) {
        scrollfire.style.width = "90%";
        scrollfire.style.top = "90px";
    }
    else {
        scrollfire.style.width = "80%";
        scrollfire.style.top = "150px";
    }

    parallax.style.backgroundPositionY = (window.scrollY*-0.3) + "px";
}

// form validation
var nameBox = document.getElementById("name");
var email = document.getElementById("email");
var reason = document.getElementById("reason");
var productArea = document.getElementById("productarea");
var productBox = document.getElementById("productbox");
var messageBox = document.getElementById("messagearea");
var formbutton = document.getElementById("formbutton");
var emailRegex = /^.+@.+\..+$/
var productRegex = /^[A-C]{1}[1-2]{1}$/

function validate() {
    var good = true;
    // name
    if (nameBox.value.length >= 4) nameBox.style.borderColor = "black";
    else {
        nameBox.style.borderColor = "red";
        good = false;
    }

    // email
    if (emailRegex.test(email.value)) email.style.borderColor = "black";
    else {
        email.style.borderColor = "red";
        good = false;
    }

    // product
    if (reason.selectedIndex == 2) {
        productArea.style.display = "block";

        if (productRegex.test(productBox.value)) productBox.style.borderColor = "black";
        else {
            productBox.style.borderColor = "red";
            good = false;
        }
    }
    else productArea.style.display = "none";

    // mesasge box
    if (messageBox.value.length > 10 && messageBox.value.length < 30) messageBox.style.borderColor = "black";
    else {
        messageBox.style.borderColor = "red";
        good = false;
    }

    // show submit button if all is a-ok
    if (good) formbutton.style.display = "block";
    else formbutton.style.display = "none";
}