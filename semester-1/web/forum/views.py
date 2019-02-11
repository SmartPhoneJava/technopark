# -*- coding: utf-8 -*-
from __future__ import unicode_literals

from django.shortcuts import render, get_object_or_404, render_to_response
from .models import Question, Answer, Profile, Tag
from .forms import PostForm, loginForm, AnswerForm
from django.utils import timezone
from django.template import Template, Context, RequestContext, loader
from django.shortcuts import redirect
from django.contrib.auth import logout
from django.contrib.auth import authenticate, login
from django.http import HttpResponseRedirect, HttpResponseForbidden, HttpResponse
import os
from django.views.generic.edit import FormView
from django.contrib.auth.forms import UserCreationForm

from django.core.paginator import Paginator, EmptyPage, PageNotAnInteger

from django.views.generic import ListView, DetailView

from django.contrib.auth.decorators import login_required, permission_required

from taggit_labels.widgets import LabelWidget

from django.views.generic.edit import UpdateView, CreateView

from django.views.decorators.csrf import csrf_exempt

from collections import Counter

def getTags():
    tags = Tag.objects.all()
    s = []
    for tag in tags:
        s.append(tag.name)
    Counter(s).most_common(10)
    return Counter(s).most_common(10)

def getFirst():
    tags = getTags()
    s = []
    for tag in tags:
        if len(tag[0]):
            s.append(tag[0])
    return s

def getSecond():
    tags = getTags()
    s = []
    m = tags[0][1]
    for tag in tags:
        s.append(tag[1] * 10 / m)
    return s

def proc(request):
    profile = ''

    if request.user.is_authenticated:
        profile = get_object_or_404(Profile, user=request.user)


    return {'app': 'askWorld',
            'user': request.user,
            'ip_address': request.META['REMOTE_ADDR'],
            'profile': profile,
            'tags_list': getFirst()}

class QuestionsListView(ListView):
    context_object_name = 'questions'
    model = Question
    template_name = 'forum/post_list.html'

    def get_context_data(self, **kwargs):
        context = super(QuestionsListView, self).get_context_data(**kwargs)
        # Add in a QuerySet of all the books
        if self.request.user.is_authenticated:
            context['profile'] = get_object_or_404(Profile, user=self.request.user)
        context['TAG'] = self.request.GET.get('tag')

        if (context['TAG'] is None or context['TAG'] == 'None'):
            context['TAG'] = ''
        context['PAGE'] = self.request.GET.get('page')
        context['SORT'] = self.request.GET.get('sort')
        if 'message' in self.request.session:
            context['message'] = self.request.session['message']
            self.request.session['message'] = ''

        #if (context['SORT'] is None or
        #        context['SORT'] != 'views' or
        #        context['SORT'] != 'comments'):
        #    context['SORT'] = 'time'

        context['tags_list'] = getFirst()

        #context['tag'] = Question.tags.most_common()
        return context

    def get_queryset(self):
        tag = self.request.GET.get('tag')

        if tag is None or  tag == '':
            questions = super().get_queryset()
        else:
            questions1 = super().get_queryset().all()
            questions = list(filter(lambda x: x.hasTag(tag), questions1))

        sort = self.request.GET.get('sort')
        if (sort == 'views'):
            questions = sorted(questions, key=lambda t: -t.views)
        elif (sort == 'comments'):
            questions = sorted(questions, key=lambda t: -t.getAnswersAmount())
        else:
            questions = sorted(questions, key=lambda t: timezone.now() - t.created_date)


        paginator = Paginator(questions, 20)
        page = self.request.GET.get('page')
        try:
            questions = paginator.page(page)
        except PageNotAnInteger:
            # If page is not an integer, deliver first page.
            questions = paginator.page(1)
        except EmptyPage:
            # If page is out of range (e.g. 9999), deliver last page of results.
            questions = paginator.page(paginator.num_pages)
        return questions

class AnswerCreate(CreateView):
    form_class = AnswerForm
    template_name = 'forum/post_page.html'
    succes_url = '/success/'

    def form_valid(self, form):
        instance = form.save(commit=False)
        profile = get_object_or_404(Profile, user=self.request.user)
        instance.author = profile
        instance.is_right = False
        instance.question = self.request.session['question']
        instance.rating = 0

        Answer.objects.create(**form.cleaned_data)
        return redirect(self.get_success_url())

def setRightAnswer(request, id):
    if request.method == 'POST':
        print('123123123')
        if not request.user.is_authenticated:
            request.session['message'] = "Комментарии могут создать только авторизованные пользователи "
        else:
            answer = Answer.objects.filter(pk=id)
            if len(answer) == 1:
                answer=answer[0]
                if answer.is_right:
                    answer.is_right = False
                else:
                    answer.is_right = True
                answer.save()

    return HttpResponseRedirect(request.META.get('HTTP_REFERER'))

class QuestionDetails(DetailView):
    context_object_name = 'question'
    model = Question
    template_name = 'forum/post_page.html'


    def post(self, request, *args, **kwargs):
        if not self.request.user.is_authenticated:
            request.session['message'] = "Комментарии могут создать только авторизованные пользователи "
        else:
            self.object = self.get_object()

            form = AnswerForm(request.POST)
            if form.is_valid():
                profile = get_object_or_404(Profile, user=self.request.user)
                answer = Answer(author=profile, is_right=False,
                                question=self.object,
                                content='',
                                rating=0)
                answer.content = form.cleaned_data['content']
                answer.save()
                request.session['message'] = "Комментарий создан"
            else:
                request.session['message'] = "Ошибка при создании комментария"

        return HttpResponseRedirect(self.request.META.get('HTTP_REFERER'))

    def post(self, request, id):
        print('123123123')
        ''' 
        if not self.request.user.is_authenticated:
            request.session['message'] = "Комментарии могут создать только авторизованные пользователи "
        else:
            answer = Answer.objects.filter(pk=id)
            if answer.is_right:
                answer.is_right = False
            else:
                answer.is_right = True
            answer.save()
        '''
        return HttpResponseRedirect(self.request.META.get('HTTP_REFERER'))

    def get_context_data(self, **kwargs):
        context = super(QuestionDetails, self).get_context_data(**kwargs)

        # Add in a QuerySet of all the books
        if self.request.user.is_authenticated:
            context['profile'] = get_object_or_404(Profile, user=self.request.user)
        answers = Answer.objects.filter(question = self.get_object())
                                                  # get_object_or_404(Question, pk=self.kwargs['pk']))
        paginator = Paginator(answers, 10)
        page = self.request.GET.get('page')
        try:
            answers = paginator.page(page)
        except PageNotAnInteger:
            # If page is not an integer, deliver first page.
            answers = paginator.page(1)
        except EmptyPage:
            # If page is out of range (e.g. 9999), deliver last page of results.
            answers = paginator.page(paginator.num_pages)

        you = False

        if self.request.user.is_authenticated:
            profile = get_object_or_404(Profile, user=self.request.user)
            if (profile == self.object.author):
                you = True

        self.get_object().upView()
        self.get_object().save()
        context['answers'] = answers
        context['details'] = True
        context['message'] = ''
        context['you'] = you
        context['tags_list'] = getFirst()

        if 'message' in self.request.session:
            context['message'] = self.request.session['message']
            self.request.session['message'] = ''

        if self.request.method == "POST":
            form = AnswerForm(self.request.POST)
        else:
            form = AnswerForm()

        context['form'] = form

        return context

@login_required
def post_new(request):
    if request.method == "POST":
        form = PostForm(request.POST)
        profile = get_object_or_404(Profile, user=request.user)
        if form.is_valid():
            post = Question(author = profile)
            post.publish()
            post.headline = form.cleaned_data['headline']
            post.content = form.cleaned_data['content']
            m_tags = form.cleaned_data['tags'].split()
            i = 0
            for m_tag in m_tags:
                tag = get_object_or_404(Tag, pk=post.getTags()[i].pk)
                tag.name = m_tag
                tag.save()
                i = i + 1
            while (i < 3):
                tag = get_object_or_404(Tag, pk=post.getTags()[i].pk)
                tag.name = ""
                tag.save()
                i = i + 1
            post.save()
            request.session['message'] = "Вопрос создан"
            return redirect('post_detail', pk=post.pk)
    else:
        form = PostForm()

    context = {'form': form}
    context.update(proc(request))
    context.update({'media': form.media})

    return render(request,'forum/post_edit.html', context)

@login_required
def post_edit(request, pk):
    post = get_object_or_404(Question, pk=pk)
    context = {}
    if (post.author.user != request.user):

        request.session['message'] = "Предотвращена попытка редактирования"
        return redirect('post_detail', pk=post.pk)

    if request.method == "POST":
        form = PostForm(request.POST, instance=post)
        if form.is_valid():
            form.save()
            request.session['message'] = "Вопрос отредактирован"
            return redirect('post_detail', pk=post.pk)
        else:
            context['message'] = "Произошла ошибка при редактировании"
    else:
        form = PostForm(instance=post)
        context['message'] = request.session['message']

    form.tags = ''
    context['form'] = form
    context.update(proc(request))

    return render(request, 'forum/post_edit.html', context)

def logout_view(request):
    logout(request)
    request.session['message'] = "Произведен вызход из аккаунта!";
    return redirect('post_list')

def login_check(request):
    if request.method == 'POST':
        # create a form instance and populate it with data from the request:
        form = loginForm(request.POST)
        #if form.is_valid():
        username = request.POST['username']
        password = request.POST['password']

        user = authenticate(username=username, password=password)
        if user is not None:
            if user.is_active:
                login(request, user)
                request.session['message'] = "Произведен вход в аккаунт!";
                return redirect('post_list')
                # Redirect to a success page.
            else:
                return render(request, 'forum/login_page.html', {'form': form, 'error': -1})
        else:
            return render(request, 'forum/login_page.html', {'form': form, 'error': -2})

    else:
        form = loginForm()

    return render(request, 'forum/login_page.html', {'form': form, 'error': -0})

def change_profile(request, pk) :

    profile_show = get_object_or_404(Profile, pk=pk)

    print(profile_show.avatar)

    answers = Answer.objects.filter(author=profile_show)

    questions = Question.objects.filter(author=profile_show)

    len_answers = len(answers)
    len_questions = len(questions)

    you = False
    profile = None

    profile_show.upView()
    profile_show.save()

    if request.user.is_authenticated:
        profile = get_object_or_404(Profile, user=request.user)

        if (profile == profile_show):
            you = True

    paginator = Paginator(answers, 5)
    page = request.GET.get('page')
    try:
        answers = paginator.page(page)
    except PageNotAnInteger:
        # If page is not an integer, deliver first page.
        answers = paginator.page(1)
    except EmptyPage:
        # If page is out of range (e.g. 9999), deliver last page of results.
        answers = paginator.page(paginator.num_pages)

    paginator = Paginator(questions, 5)
    page = request.GET.get('page')
    try:
        questions = paginator.page(page)
    except PageNotAnInteger:
        # If page is not an integer, deliver first page.
        questions = paginator.page(1)
    except EmptyPage:
        # If page is out of range (e.g. 9999), deliver last page of results.
        questions = paginator.page(paginator.num_pages)


    context = {'profile': profile, 'you': you,
            'profile_show': profile_show,
              'answers': answers,
              'questions': questions,
               'answers_size': len_answers,
               'questions_size': len_questions}
    context.update(proc(request))

    '''
    if request.method == 'POST':
        form = ProfileForm(request.POST)
        print('sooome' + request.POST['avatar'].url)
        profile[0].avatar = request.POST['avatar']
        profile[0].save()
        print('sooome' + profile[0].avatar.url)
    else:
        form = ProfileForm(instance=profile)
    print('profile:' + profile[0].getImage())
    '''
    return render(request, 'forum/profile.html', context)

def upload_pic(request):
    if request.method == 'POST':
        if 'some' in request.POST:
            filename = request.POST['some']
        else:
            filename = None
        if filename is not None:
            profile = Profile.objects.filter(user=request.user)
            profile[0].setImage(filename)
            profile[0].save()
            return change_profile(request, profile[0].pk)
    #else
    #    return HttpResponseForbidden('allowed only via POST')
    files = os.listdir(os.getcwd() + '/media/images/')

    context = {'files': files}
    context.update(proc(request))

    return render(request, 'forum/upload_image.html', context)

class RegisterFormView(FormView):
    form_class = UserCreationForm
    success_url = "'login/page'"

    template_name = "forum/register.html"

    def form_valid(self, form):
        form.save()
        return redirect('login_page')

def handler404(request):
    response = render_to_response('404.html', {},
                                  context_instance=RequestContext(request))
    response.status_code = 404
    return response


def handler500(request):
    response = render_to_response('500.html', {},
                                  context_instance=RequestContext(request))
    response.status_code = 500
    return response

