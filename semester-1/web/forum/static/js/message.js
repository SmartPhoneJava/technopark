var start = Date.now(); // сохранить время начала

var timer = setInterval(function() {
  // вычислить сколько времени прошло с начала анимации
  var timePassed = Date.now() - start;

  if (timePassed >= 10000) {
    clearInterval(timer); // конец через 2 секунды
    return;
  }

  // рисует состояние анимации, соответствующее времени timePassed
  draw(timePassed);

}, 20);

mess = document.getElementById("topNavLine");
if (mess.textContent) {
    mess.style.opacity = 1
}

// в то время как timePassed идёт от 0 до 2000
// left принимает значения от 0 до 400px
function draw(timePassed) {
    mess = document.getElementById("topNavLine");
    //alert(mess.innerHTML);
    mess.style.opacity = mess.style.opacity - 0.005;
}