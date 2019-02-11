from django import forms

from .models import Question, Profile, Tag, Answer
from django.contrib.auth.models import User
from taggit_labels.widgets import LabelWidget
from taggit.forms import *
from django.shortcuts import get_object_or_404

class loginForm(forms.ModelForm):
    class Meta:
        model = User
        fields = ('username', 'password')

class ProfileForm(forms.ModelForm):

    class Meta:
        model = Profile
        fields = ('avatar',)

class AnswerForm(forms.ModelForm):
    class Meta:
        model = Answer
        fields = ('content',)

class PostForm(forms.ModelForm):

    tags = forms.CharField(max_length=100)

    class Meta:
        model = Question
        fields = ('headline', 'content', 'tags')

    def clean(self):
        m_tags = self.cleaned_data['tags'].split()
        if (len(m_tags) > 3 or len(m_tags) == 0):
            raise forms.ValidationError("Максимально допустимое количество тегов: 3")

    def save(self):
        post = super(PostForm, self).save(commit=False)
        m_tags = self.cleaned_data['tags'].split()
        if (len(m_tags) < 4 and len(m_tags) > 0):
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
        return post.save()


    '''
    mail = forms.CharField(label='InputEmail', max_length=100)
    password = forms.CharField(label='InputPassword', max_length=100)
    '''

class ImageUploadForm(forms.Form):
    image = forms.ImageField()