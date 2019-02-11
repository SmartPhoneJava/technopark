from django.core.management.base import BaseCommand
from django.contrib.auth.models import User
from django.contrib.auth.hashers import make_password
from forum.models import Question, Tag, Answer, Profile
from random import choice, randint
from faker import Factory


class Command(BaseCommand):
    help = 'Fill questions'


    def add_arguments(self, parser):
        parser.add_argument('--number',
                            action='store',
                            dest='number',
                            default=10,
                            help='Number of answers to add'
                            )

    def handle(self, *args, **options):
        fake_factory = Factory.create('en_US')  # Initializes fake factory
        number = int(options['number'])  # Number of users to generate

        users = list(Profile.objects.all())
        questions = list(Question.objects.all())
        questions_len = len(questions)

        tags_list = [fake_factory.words(5), fake_factory.words(5), fake_factory.words(5)]

        for q in questions:
            num_ans = randint(0, 50)
            for j in range(num_ans):
                answer = Answer.objects.create(author = choice(users),
                                               question = q,
                                                is_right = False,
                                                content = fake_factory.text(max_nb_chars=500),
                                                  rating = 0)

                answer.save()