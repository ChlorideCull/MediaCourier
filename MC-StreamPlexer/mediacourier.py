#!/usr/bin/python3
import os.path
import sys
import argparse

def print_status(leftstring, isgood, rightstring="\u2588"):
    rharray = ["\x1B[31m", rightstring, "\x1B[39m"]
    if isgood:
        rharray[0] = "\x1B[32m"
    print("{:<30}{:>30}".format(leftstring, "".join(rharray)))

def init_parser_for_action(actionsel, actiondesc):
    argparser = argparse.ArgumentParser(description="Manage the MediaCourier Installation.")
    argparser.add_argument('action', metavar=actionsel, action='store_const', const=actionsel, help=actiondesc)
    
def parse_all_args():
    defined_actions=["health", "start-publish", "finalize-recording"]
    argparser = argparse.ArgumentParser(description="Manage the MediaCourier Installation.",
        epilog="Help for individual Actions can be retrieved by calling them with \"--help\".")
    argparser.add_argument('action', metavar='<action>', help="Action to perform, such as \"health\".", choices=defined_actions)
    argparser.add_argument('arguments', metavar='<action arguments>', help="Arguments to the Action in question.", nargs=argparse.REMAINDER)
    firststage = argparser.parse_args()
    
    if firststage.action == "health":
        argparser = init_parser_for_action(firststage.action, "Check the health of the MediaCourier instance.")
    elif firststage.action == "start-publish":
        argparser = init_parser_for_action(firststage.action, "Flag a streamer as streaming, updating the database properly.")
        argparser.add_argument('name', metavar='<streamer name>', help="Name of the streamer who will be flagged as streaming.")
    elif firststage.action == "finalize-recording":
        argparser = init_parser_for_action(firststage.action, "Finalize recordings, converting them to a smaller format.")
        argparser.add_argument('path', metavar='<stream recordings>', help="Path to the recordings generated.")
    return argparser.parse_args()
    
def main():
    argv = parse_all_args()
    
    if (argv.action == "health"):
        print_status("Is nginx running?", os.path.isfile("/var/run/mediacourier_nginx.pid"))

main()
