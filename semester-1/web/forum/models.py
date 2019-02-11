from django.db import models
from django.utils import timezone

from django.db import models
from django.utils import timezone
from django.contrib.auth.models import User
from django.dispatch import receiver
from django.db.models.signals import post_save
from taggit_autosuggest.managers import TaggableManager

import os


class Tag(models.Model):
    name = models.TextField(blank=True)

class Profile(models.Model):
    user = models.OneToOneField(User, on_delete=models.CASCADE)

    avatar = models.TextField(default="cat.png")

    '''
    default =
    ImageFile(open(os.getcwd() + "/media/images/cat.png", "rb")))
    '''''

    registration_date = models.DateTimeField(
           default=timezone.now, blank=True)
    rating = models.IntegerField(default = 0, blank=True)
    views = models.IntegerField(default=0)

    def __str__(self):
        return self.user.username

    def __unicode__(self):
        return self.avatar

    def getImage(self):
        return self.avatar

    def setImage(self, file):
        self.avatar = file
        self.save()

    def upView(self):
        self.views = self.views + 1
        self.save()

    def bit(self):
        if self.avatar:
            return u'<img src="%s" width="70"/>' % self.avatar.url
        else:
            return u'(none)'

@receiver(post_save, sender =User)
def create_user_profile(sender, instance, created, **kwargs):
    user = instance
    if created:
        profile = Profile(user=user)
        profile.save()

@receiver(post_save, sender =User)
def save_user_profile(sender, instance, **kwargs):
    instance.profile.save()

class Question(models.Model):
    headline = models.TextField()
    content = models.TextField()
    author = models.ForeignKey('Profile', on_delete = models.CASCADE)
    created_date = models.DateTimeField(default = timezone.now)

    tags = models.ManyToManyField('Tag')

    rating = models.IntegerField
    views = models.IntegerField(default=0)
    
    def publish(self):
        self.created_date = timezone.now()
        self.save()
        tag1 = Tag(name="123")
        tag1.save()
        self.tags.add(tag1)
        self.save()
        tag2 = Tag(name="454")
        tag2.save()
        self.tags.add(tag2)
        self.save()
        tag3 = Tag(name="343")
        tag3.save()
        self.tags.add(tag3)
        self.save()

    def __str__(self):
        return self.headline

    def upView(self):
        self.views = self.views + 1
        self.save()

    def getAnswersAmount(self):
        return len(Answer.objects.filter(question=self))

    def getTags(self):
        return self.tags.all()

    def getTagsNames(self):
        s = []
        for tag in self.tags.all():
            s.append(tag.name)
        return s

    def hasTag(self, find_tag):
        for tag in self.tags.all():
            if (tag.name == find_tag):
                return True
        return False


class Answer(models.Model):
    content = models.TextField()
    author = models.ForeignKey('Profile', on_delete = models.CASCADE)
    question = models.ForeignKey('Question', on_delete = models.CASCADE)
    created_date = models.DateTimeField(default = timezone.now)
    is_right = models.BooleanField()
    rating = models.IntegerField()

    def setContent(self, content):
        self.content = content
        self.save()

    def publish(self):
        self.created_date = timezone.now()
        self.save()

    def __str__(self):
        return self.content