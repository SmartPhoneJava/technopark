from django.core.management.base import BaseCommand
from django.contrib.auth.models import User
from django.contrib.auth.hashers import make_password
from forum.models import Question, Tag, Answer, Profile
from random import choice, randint
from faker import Factory
from django.shortcuts import get_object_or_404


class Command(BaseCommand):
    help = 'Fill questions'

    def add_arguments(self, parser):
        parser.add_argument('--number',
                            action='store',
                            dest='number',
                            default=100,
                            help='Number of questions to add'
                            )

    def handle(self, *args, **options):
        fake_factory = Factory.create('en_US')  # Initializes fake factory
        number = int(options['number'])  # Number of users to generate

        users = Profile.objects.all()

        tags_list = [fake_factory.words(5), fake_factory.words(5), fake_factory.words(5)]

        for i in range(number):
            question = Question.objects.create(author=choice(users))
            question.headline = fake_factory.sentence(nb_words=randint(2, 4), variable_nb_words=True)
            question.content = fake_factory.text(max_nb_chars=500)
            question.created_date = fake_factory.date()
            question.publish()
            question.save()
            pk = question.pk

            num_tags = randint(1, 2)
            for j in range(num_tags):
                new_tag = get_object_or_404(Tag, pk=question.getTags()[j].pk)
                new_tag.name = choice(tags_list[j])
                new_tag.save()
                question.save()