
buttons_edit = document.getElementsByClassName("orange_20pt glyphicon glyphicon-pencil");
buttons_delete = document.getElementsByClassName("orange_20pt glyphicon glyphicon-remove");
buttons_accept = document.getElementsByClassName("orange_20pt glyphicon glyphicon-ok");

button_question_edit = document.getElementById("edit question button");

user_answers_nickname = document.getElementsByClassName("boxf nickname");
your_nickname = document.getElementById("your_name");
author_nickname = document.getElementsByClassName("box_q nickname_q")[0];

for (i = 0; i < user_answers_nickname.length; i++) {
    var str_answer = user_answers_nickname[i].textContent;
    var str_you = your_nickname.textContent;
    if (str_answer.trim() != str_you.trim())
    {
        buttons_edit[i].style.display = "none";
        buttons_delete[i].style.display = "none";

    }
    else
    {
        buttons_edit[i].style.display = "block";
        buttons_delete[i].style.display = "block";
    }
    var str_quastion = author_nickname.textContent;
    if (str_quastion.trim() != str_you.trim())
    {
        buttons_accept[i].style.display = "none";
    }
    else
    {
        buttons_accept[i].style.display = "block";
    }
}

var s_you = your_nickname.textContent.trim();
var s_author = author_nickname.textContent.trim();

if (s_you != s_author) {
    button_question_edit.style.display = "none";
} else {
    button_question_edit.style.display = "block";
}

function best_ans()
{
    var right = $(this);
    var id = right.data('id_answer');

    $.ajax({
    url : "/right" + "/" + id + "/",
    type : 'POST',
    data : { 'obj1' : id },

    success : function (json) {
    right.find("[data-count='best_ans']").text = 'none'
    $(this).style.display = 'none'
    }
    });

    return false;
}

$(function() {

$('[data-action="best_ans"]').click(best_ans);
});
