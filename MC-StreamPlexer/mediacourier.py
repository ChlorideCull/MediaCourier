#!/usr/bin/python3
import os.path
import argparse

def print_status(leftstring, isgood, rightstring="\u2588"):
    rharray = ["\x1B[31m", rightstring, "\x1B[39m"]
    if isgood:
        rharray[0] = "\x1B[32m"
    print("{:<30}{:>30}".format(leftstring, "".join(rharray)))

def main():
    defined_actions=["health"]

    argparser = argparse.ArgumentParser(description="Manage the MediaCourier Installation.")
    argparser.add_argument('action', metavar='<action>', help="Action to perform, such as \"health\".", choices=defined_actions)
    argv = argparser.parse_args()
    
    if (argv.action == "health"):
        print_status("Is nginx running?", os.path.isfile("/var/run/mediacourier_nginx.pid"))
        