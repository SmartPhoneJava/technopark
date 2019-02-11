from django.conf.urls import url
from . import views
from django.contrib.staticfiles.urls import staticfiles_urlpatterns
from django.conf.urls.static import static
from django.conf import settings
from django.contrib.auth.decorators import login_required

urlpatterns = [
    url(r'^post/(?P<pk>\d+)/$', views.QuestionDetails.as_view(), name='post_detail'),
    url(r'^post/new/$', views.post_new, name='post_new'),
    url(r'^post/(?P<pk>\d+)/edit/$', views.post_edit, name='post_edit'),
    url(r'^right/(?P<id>\d+)/$', views.setRightAnswer, name='right_answer'),
    url(r'^post/(?P<pk>\d+)/comment/create$', views.AnswerCreate.as_view(), name='answer_create'),
    url(r'^logout$', views.logout_view, name='logout'),
    url(r'^login/page$', views.login_check, name='login_page'),
    url(r'^profile/page/(?P<pk>\d+)/$', views.change_profile, name='profile_page'),
    url(r'^profile/change_photo$', views.upload_pic, name='change_photo'),
    url(r'^upload_image$', views.upload_pic, name='change_photo'),
    url(r'^register$', views.RegisterFormView.as_view(), name='register'),
    url(r'^$', views.QuestionsListView.as_view(), name='post_list'),

]

handler404 = 'forum.views.handler404'
handler500 = 'forum.views.handler500'

if settings.DEBUG:
    urlpatterns += staticfiles_urlpatterns() + static(
    settings.MEDIA_URL, document_root=settings.MEDIA_ROOT)
    urlpatterns += static(settings.STATIC_URL, document_root=settings.STATIC_ROOT)
    urlpatterns += static(settings.MEDIA_URL, document_root=settings.MEDIA_ROOT)
