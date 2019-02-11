# -*- coding: utf-8 -*-
from django.contrib import admin
from .models import Profile
from .models import Answer
from .models import Question
from .models import Tag

admin.site.register(Profile)
admin.site.register(Answer)
admin.site.register(Question)
admin.site.register(Tag)
