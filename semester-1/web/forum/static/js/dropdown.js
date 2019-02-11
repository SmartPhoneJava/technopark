/* When the user clicks on the button,
toggle between hiding and showing the dropdown content */
function myFunction() {
    document.getElementById("myDropdown").classList.toggle("show");
}


if (document.getElementsByClassName("dropbtn")[0].textContent == "views") {
    document.getElementsByClassName("dropbtn")[0].textContent = "Популярности";
}

else if (document.getElementsByClassName("dropbtn")[0].textContent == "comments") {
    document.getElementsByClassName("dropbtn")[0].textContent = "Количеству ответов";
}

else {
    document.getElementsByClassName("dropbtn")[0].textContent = "Времени";
}

// Close the dropdown menu if the user clicks outside of it
window.onclick = function(event) {
  if (!event.target.matches('.dropbtn')) {

    var dropdowns = document.getElementsByClassName("dropdown-content");
    var i;
    for (i = 0; i < dropdowns.length; i++) {
      var openDropdown = dropdowns[i];
      if (openDropdown.classList.contains('show')) {
        openDropdown.classList.remove('show');
      }
    }
  }
}